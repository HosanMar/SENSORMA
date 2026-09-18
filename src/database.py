from sqlalchemy import create_engine, Column, Integer, Numeric, DateTime
from sqlalchemy.orm import declarative_base, sessionmaker
from datetime import datetime

# Substitua com seus dados reais do PostgreSQL
SQLALCHEMY_DATABASE_URL = "postgresql://postgres:postgres@localhost:5432/sensorma"

engine = create_engine(SQLALCHEMY_DATABASE_URL)
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)
Base = declarative_base()

class Leitura(Base):
    __tablename__ = "leituras_sensores"
    id = Column(Integer, primary_key=True, index=True)
    temperatura = Column(Numeric(5,2))
    umidade = Column(Numeric(5,2))
    ruido = Column(Integer)
    voc = Column(Numeric(8,2))
    co = Column(Numeric(8,2))
    dust = Column(Numeric(8,2))
    light = Column(Integer)
    data_hora = Column(DateTime, default=datetime.utcnow)