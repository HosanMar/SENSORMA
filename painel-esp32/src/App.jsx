import { useState, useEffect } from 'react';
import axios from 'axios';
import { 
  LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, ResponsiveContainer, Brush 
} from 'recharts';
import { Thermometer, Droplet, Flame, Activity, Clock } from 'lucide-react';

function App() {
  const [leituras, setLeituras] = useState([]);
  const [leituraAtual, setLeituraAtual] = useState(null);

  useEffect(() => {
    const fetchLeituras = async () => {
      try {
        // Atualize com o IP correto se necessário
        const response = await axios.get('http://10.0.0.59/leituras');
        
        if (response.data && response.data.length > 0) {
          // Salva a leitura mais recente (a primeira do array) para os Cards
          setLeituraAtual(response.data[0]);

          const dadosFormatados = response.data.reverse().map(item => ({
            ...item,
            hora_formatada: new Date(item.data_hora).toLocaleTimeString([], { hour: '2-digit', minute: '2-digit', second: '2-digit' }),
            data_curta: new Date(item.data_hora).toLocaleDateString([], { day: '2-digit', month: 'short' })
          }));
          
          setLeituras(dadosFormatados);
        }
      } catch (error) {
        console.error("Erro ao buscar dados da API:", error);
      }
    };

    fetchLeituras();
    const intervalo = setInterval(fetchLeituras, 60000);
    return () => clearInterval(intervalo);
  }, []);

  // --- Estilos Baseados no ThingsBoard ---
  const styles = {
    container: { backgroundColor: '#eceff1', minHeight: '100vh', padding: '16px', fontFamily: '"Roboto", "Helvetica", sans-serif', color: '#333' },
    gridPrincipal: { display: 'grid', gridTemplateColumns: '1fr 2fr', gap: '16px', marginBottom: '16px' },
    gridSecundario: { display: 'grid', gridTemplateColumns: 'repeat(3, 1fr)', gap: '16px', marginBottom: '16px' },
    card: { backgroundColor: '#fff', border: '1px solid #cfd8dc', borderRadius: '4px', padding: '16px', boxShadow: '0 1px 3px rgba(0,0,0,0.05)', display: 'flex', flexDirection: 'column' },
    cardHeader: { display: 'flex', alignItems: 'flex-start', gap: '12px', marginBottom: 'auto' },
    iconBox: { backgroundColor: '#e3f2fd', color: '#1e88e5', padding: '12px', borderRadius: '50%', display: 'flex' },
    titleInfo: { display: 'flex', flexDirection: 'column' },
    title: { fontSize: '1.2rem', fontWeight: '500', margin: 0 },
    subtitle: { fontSize: '0.85rem', color: '#78909c', marginTop: '2px' },
    valueMain: { fontSize: '4rem', fontWeight: '600', textAlign: 'center', marginTop: '30px' },
    valueSub: { fontSize: '2.5rem', fontWeight: '500', textAlign: 'right', marginTop: '16px', display: 'flex', alignItems: 'baseline', justifyContent: 'flex-end', gap: '8px' },
    unit: { fontSize: '1.2rem', color: '#546e7a', fontWeight: '400' },
    tableHeader: { backgroundColor: '#f9f9f9', borderBottom: '2px solid #eceff1', textAlign: 'left', padding: '12px', color: '#546e7a', fontSize: '0.9rem', fontWeight: '500' },
    tableCell: { padding: '12px', borderBottom: '1px solid #eceff1', fontSize: '0.9rem' }
  };

  const ultimaAtt = leituraAtual ? `Última atualização: ${new Date(leituraAtual.data_hora).toLocaleTimeString()}` : 'Aguardando dados...';

  return (
    <div style={styles.container}>
      
      {/* LINHA 1: Temperatura Principal e Gráfico */}
      <div style={styles.gridPrincipal}>
        {/* Card Temperatura */}
        <div style={styles.card}>
          <div style={styles.cardHeader}>
            <div style={{...styles.iconBox, backgroundColor: '#e8eaf6', color: '#3f51b5'}}>
              <Thermometer size={28} />
            </div>
            <div style={styles.titleInfo}>
              <h3 style={styles.title}>Temperature</h3>
              <span style={styles.subtitle}>{ultimaAtt}</span>
            </div>
          </div>
          <div style={styles.valueMain}>
            {leituraAtual?.temperatura ?? 'nan'} <span style={{fontSize:'2rem'}}>°C</span>
          </div>
        </div>

        {/* Card Gráfico */}
        <div style={styles.card}>
          <h3 style={{...styles.title, marginBottom: '4px'}}>Line chart</h3>
          <span style={{...styles.subtitle, marginBottom: '20px'}}>Tempo real - últimas leituras</span>
          <div style={{ height: '300px' }}>
            <ResponsiveContainer width="100%" height="100%">
              <LineChart data={leituras} margin={{ top: 5, right: 30, left: -20, bottom: 5 }}>
                <CartesianGrid strokeDasharray="3 3" vertical={false} stroke="#eceff1" />
                <XAxis dataKey="data_curta" tick={{fontSize: 12, fill: '#78909c'}} tickLine={false} axisLine={{stroke: '#cfd8dc'}} />
                <YAxis tick={{fontSize: 12, fill: '#78909c'}} tickLine={false} axisLine={false} />
                <Tooltip />
                <Line type="monotone" dataKey="temperatura" stroke="#2196f3" strokeWidth={2} dot={false} name="Temperatura" />
                {/* O Brush cria a barra de zoom azul embaixo do gráfico */}
                <Brush dataKey="hora_formatada" height={30} stroke="#90caf9" fill="#f1f8ff" travellerWidth={10} />
              </LineChart>
            </ResponsiveContainer>
          </div>
        </div>
      </div>

      {/* LINHA 2: Métricas Secundárias */}
      <div style={styles.gridSecundario}>
        {/* Card Umidade */}
        <div style={{...styles.card, flexDirection: 'row', alignItems: 'center', justifyContent: 'space-between'}}>
          <div style={styles.cardHeader}>
            <div style={{...styles.iconBox, color: '#00bcd4', backgroundColor: '#e0f7fa'}}><Droplet size={24} /></div>
            <div style={styles.titleInfo}>
              <h3 style={styles.title}>Umidade</h3>
              <span style={styles.subtitle}>{ultimaAtt}</span>
            </div>
          </div>
          <div style={styles.valueSub}>{leituraAtual?.umidade ?? 'nan'}</div>
        </div>

        {/* Card CO2 / VOC */}
        <div style={{...styles.card, flexDirection: 'row', alignItems: 'center', justifyContent: 'space-between'}}>
          <div style={styles.cardHeader}>
            <div style={{...styles.iconBox, color: '#f44336', backgroundColor: '#ffebee'}}><Flame size={24} /></div>
            <div style={styles.titleInfo}>
              <h3 style={styles.title}>VOC</h3>
              <span style={styles.subtitle}>{ultimaAtt}</span>
            </div>
          </div>
          <div style={styles.valueSub}>
            {leituraAtual?.voc ?? '0'} <span style={styles.unit}>ppm</span>
          </div>
        </div>

        {/* Card CO */}
        <div style={{...styles.card, flexDirection: 'row', alignItems: 'center', justifyContent: 'space-between'}}>
          <div style={styles.cardHeader}>
            <div style={{...styles.iconBox, color: '#4caf50', backgroundColor: '#e8f5e9'}}><Activity size={24} /></div>
            <div style={styles.titleInfo}>
              <h3 style={styles.title}>CO</h3>
              <span style={styles.subtitle}>{ultimaAtt}</span>
            </div>
          </div>
          <div style={styles.valueSub}>
            {leituraAtual?.co ?? '0'} <span style={styles.unit}>mg/m³</span>
          </div>
        </div>
      </div>

      {/* LINHA 3: Tabela */}
      <div style={styles.card}>
        <h3 style={{...styles.title, marginBottom: '4px'}}>Timeseries table</h3>
        <span style={{...styles.subtitle, display: 'flex', alignItems: 'center', gap: '6px', marginBottom: '16px'}}>
          <Clock size={14} /> Tempo real - histórico completo
        </span>
        
        <div style={{ overflowX: 'auto' }}>
          <table style={{ width: '100%', borderCollapse: 'collapse' }}>
            <thead>
              <tr>
                <th style={styles.tableHeader}>Timestamp ↓</th>
                <th style={styles.tableHeader}>Umidade</th>
                <th style={styles.tableHeader}>Temperatura</th>
                <th style={styles.tableHeader}>CO</th>
                <th style={styles.tableHeader}>VOC</th>
                <th style={styles.tableHeader}>Luminosidade</th>
              </tr>
            </thead>
            <tbody>
              {/* Inverte novamente para a tabela exibir do mais novo para o mais velho */}
              {[...leituras].reverse().map((leitura) => (
                <tr key={leitura.id}>
                  <td style={styles.tableCell}>{new Date(leitura.data_hora).toLocaleString()}</td>
                  <td style={styles.tableCell}>{leitura.umidade ?? 'nan'}</td>
                  <td style={styles.tableCell}>{leitura.temperatura ?? 'nan'}</td>
                  <td style={styles.tableCell}>{leitura.co}</td>
                  <td style={styles.tableCell}>{leitura.voc}</td>
                  <td style={styles.tableCell}>{leitura.light}</td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      </div>

    </div>
  );
}

export default App;