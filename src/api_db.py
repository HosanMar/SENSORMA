from flask import Flask, request, jsonify
import psycopg2

app = Flask(__name__)

DB_CONFIG = {
    "host": "localhost",
    "database": "sensorma",
    "user": "postgres",
    "password": "postgres"
}

@app.route('/', methods=['GET'])
def index():
    return "API está online e pronta para receber dados do ESP32!"

@app.route('/salvar', methods=['POST'])
def salvar_dados():
    dados = request.json
    
    try:
        # Conecta ao banco de dados
        conn = psycopg2.connect(**DB_CONFIG)
        cursor = conn.cursor()
        
        # Prepara a query SQL
        query = """
            INSERT INTO leituras_sensores 
            (temperatura, umidade, voc, co, light)
            VALUES (%s, %s, %s, %s, %s)
        """
        
        # Extrai os valores do JSON enviado pelo ESP32
        valores = (
            dados.get('temperatura'),
            dados.get('umidade'),
            dados.get('VOC'),
            dados.get('CO'),
            dados.get('light')
        )
        
        cursor.execute(query, valores)
        conn.commit()
        
        cursor.close()
        conn.close()
        
        return jsonify({"status": "sucesso"}), 201

    except Exception as e:
        print(f"Erro no banco: {e}")
        return jsonify({"erro": str(e)}), 500

if __name__ == '__main__':
    # Roda a API exposta na rede local para o ESP32 conseguir acessar
    app.run(host='192.168.0.11', port=5000)