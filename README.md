# Edge Computing — Health Score Care Plus
### Sprint 3 | ESP32 + MQTT + Node-RED

## Integrantes

- Eduardo Navarro Lúcio — RM: 568095
- Victor Zocoler — RM: 568234
- Guilherme Pereira — RM: 567487

## Descrição

Dispositivo vestível simulado com ESP32 que captura métricas de saúde
e envia via protocolo MQTT para o Node-RED/FIWARE.

## Arquitetura

Edge (ESP32) → MQTT Broker → Node-RED → Dashboard

## Sensores Simulados

- Frequência cardíaca
- Temperatura corporal
- Passos diários

## Links

- 🔗 Simulação Wokwi: https://wokwi.com/projects/461648932538084353?classId=81e90722-86b4-4999-8cef-4ad54a723bf4&assignmentId=c55ac295-411a-4266-8bcb-5c715d6e12c0&submissionId=9ac97504-3746-473c-8b89-f35ea058c440
- 🎥 Vídeo demonstração: https://youtube.com/shorts/Ultqg1aPEwM?is=9UUKzZS7xlIjGHlM

## Como rodar

1. Abra o link do Wokwi
2. Clique em Play para iniciar a simulação
3. Os dados são enviados via MQTT em tempo real