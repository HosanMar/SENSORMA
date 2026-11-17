from fastapi import FastAPI, Depends
from sqlalchemy.orm import Session
from pydantic import BaseModel, Field
from fastapi.middleware.cors import CORSMiddleware
from database import SessionLocal, engine, Base, Leitura

# Cria as tabelas no banco caso não existam
Base.metadata.create_all(bind=engine)

app = FastAPI(title="Monitoramento Ambiental ESP32")

# Configuração de CORS vital para o React (Frontend) não ser bloqueado ao chamar a API
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"], # Na produção, coloque a URL do seu React
    allow_methods=["*"],
    allow_headers=["*"],
)

# Validador dos dados que chegam do ESP32
class LeituraCreate(BaseModel):
    temperatura: float
    umidade: float
    Ruido: int = Field(alias="Ruído") # Pydantic lida com o acento do C++ do ESP32
    VOC: float
    CO: float
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
    nova_leitura = Leitura(
        temperatura=dados.temperatura,
        umidade=dados.umidade,
        ruido=dados.Ruido,
        voc=dados.VOC,
        co=dados.CO,
        dust=dados.dust,
        light=dados.light
    )
    db.add(nova_leitura)
    db.commit()
    return {"status": "sucesso"}

@app.get("/leituras")
def listar_leituras(limite: int = 50, db: Session = Depends(get_db)):
    # Retorna os dados ordenados dos mais recentes para os mais antigos
    return db.query(Leitura).order_by(Leitura.data_hora.desc()).limit(limite).all()