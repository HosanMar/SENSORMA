from fastapi import FastAPI, Depends
from sqlalchemy.orm import Session
from pydantic import BaseModel
from fastapi.middleware.cors import CORSMiddleware
from database import SessionLocal, engine, Base, Leitura
from datetime import datetime

# Cria as tabelas no banco caso não existam
Base.metadata.create_all(bind=engine)

app = FastAPI(title="Monitoramento Ambiental ESP32")
leitura_tempo_real = None
ultima_gravacao_banco = None

# Configuração de CORS vital para o React (Frontend) não ser bloqueado ao chamar a API
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"], # Na produção, coloque a URL do seu React
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Validador dos dados que chegam do ESP32
class LeituraCreate(BaseModel):
    temperatura: float
    umidade: float
    ruido: int
    voc: float
    co: float
    dust: float
    light: int

# Injeção de dependência do banco de dados
def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

@app.post("/salvar", status_code=201)
def salvar_dados(dados: LeituraCreate, db: Session = Depends(get_db)):
    global leitura_tempo_real, ultima_gravacao_banco
    agora = datetime.now()

    # Grava no PostgreSQL na primeira execução ou após completar 1 hora (3600s)
    if ultima_gravacao_banco is None or (agora - ultima_gravacao_banco).total_seconds() >= 3600:
        nova_leitura = Leitura(
            temperatura=dados.temperatura,
            umidade=dados.umidade,
            ruido=dados.ruido,
            voc=dados.voc,
            co=dados.co,
            dust=dados.dust,
            light=dados.light
        )
        db.add(nova_leitura)
        db.commit()
        db.refresh(nova_leitura)
        
        ultima_gravacao_banco = agora
        leitura_tempo_real = None  # Limpa a memória para não duplicar com o registro recém-salvo
        return {"status": "salvo no banco"}

    # Nos intervalos de 5 minutos, guarda apenas na memória
    leitura_tempo_real = {
        "id": 0,
        "temperatura": dados.temperatura,
        "umidade": dados.umidade,
        "ruido": dados.ruido,
        "voc": dados.voc,
        "co": dados.co,
        "dust": dados.dust,
        "light": dados.light,
        "data_hora": agora
    }
    return {"status": "atualizado em memoria"}

@app.get("/leituras")
def listar_leituras(limite: int = 50, db: Session = Depends(get_db)):
    historico = db.query(Leitura).order_by(Leitura.data_hora.desc()).limit(limite).all()
    
    # Coloca a leitura mais recente de 5 min no topo da lista (índice 0)
    if leitura_tempo_real is not None:
        return [leitura_tempo_real] + historico
        
    return historico