# IoT Data Management with Hyperledger Fabric

## Project Overview
This project demonstrates an end-to-end IoT data management system using Hyperledger Fabric blockchain. It collects sensor data from an ESP8266 with DHT11 sensor, stores it securely on a blockchain network, and provides a way for clients to access this data through the internet.

## Architecture
The system consists of three main components:
1. **IoT Device Layer**: ESP8266 with DHT11 sensor that collects temperature and humidity data
2. **Blockchain Layer**: Hyperledger Fabric network for secure data storage
3. **Application Layer**: Backend API and client interface for data access

## Features
- Secure, immutable storage of sensor data on blockchain
- Minimal Hyperledger Fabric network (1 organization, 1 peer, 1 orderer)
- RESTful API for data submission and retrieval
- Web dashboard for visualizing sensor data
- Cloud deployment ready

## Prerequisites
- Ubuntu 20.04/22.04 (tested in VirtualBox)
- Docker 27.5+ and Docker Compose
- Go 1.21+
- Node.js 16+ (for backend API)
- ESP8266 microcontroller
- DHT11 temperature/humidity sensor

## Setup Instructions

### 1. Clone the Repository

git clone https://github.com/yourusername/iot-fabric-project.git
cd iot-fabric-project

# Install Docker and Docker Compose
sudo apt update && sudo apt upgrade -y
sudo apt install -y docker.io docker-compose
sudo usermod -aG docker $USER
newgrp docker

# Install Go
wget https://go.dev/dl/go1.21.0.linux-amd64.tar.gz
sudo tar -C /usr/local -xzf go1.21.0.linux-amd64.tar.gz
echo "export PATH=$PATH:/usr/local/go/bin" >> ~/.bashrc
source ~/.bashrc

# Install Hyperledger Fabric binaries and samples
curl -sSL https://bit.ly/2ysbOFE | bash -s

cd test-network
./start-minimal-network.sh

export PATH=${PWD}/../bin:$PATH
export FABRIC_CFG_PATH=$PWD/../config/
./network.sh deployCC -ccn iotcc -ccp ../chaincode/iot -ccl go

# Set environment variables
export CORE_PEER_TLS_ENABLED=true
export CORE_PEER_LOCALMSPID="Org1MSP"
export CORE_PEER_TLS_ROOTCERT_FILE=${PWD}/organizations/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/ca.crt
export CORE_PEER_MSPCONFIGPATH=${PWD}/organizations/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp
export CORE_PEER_ADDRESS=localhost:7051

# Add sample sensor data
peer chaincode invoke -o localhost:7050 --ordererTLSHostnameOverride orderer.example.com \
--tls --cafile ${PWD}/organizations/ordererOrganizations/example.com/orderers/orderer.example.com/msp/tlscacerts/tlsca.example.com-cert.pem \
-C mychannel -n iotcc \
--peerAddresses localhost:7051 \
--tlsRootCertFiles ${PWD}/organizations/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/ca.crt \
-c '{"function":"AddSensorData","Args":["esp8266_1","28.5","65.2"]}'

# Query sensor data
peer chaincode query -C mychannel -n iotcc -c '{"Args":["QueryAllSensorData"]}'

iot-fabric-project/
├── chaincode/
│   └── iot/                  # IoT chaincode (Go)
├── config/                   # Hyperledger Fabric configuration
├── test-network/
│   ├── docker-compose-minimal.yml
│   ├── start-minimal-network.sh
│   ├── organizations/        # Crypto material
│   ├── system-genesis-block/
│   └── channel-artifacts/
├── backend/                  # Node.js backend API (to be implemented)
├── client/                   # Web dashboard (to be implemented)
└── README.md

Chaincode Functions
The IoT chaincode provides the following functions:

AddSensorData(deviceID, temperature, humidity): Stores new sensor data
QuerySensorData(key): Retrieves a specific sensor record
QueryAllSensorData(): Returns all sensor data in the ledger
Future Work
 Implement Node.js backend API with Fabric SDK
 Create web dashboard for data visualization
 Integrate ESP8266 with the backend API
 Deploy entire system to cloud infrastructure
 Add authentication and authorization for clients
 Implement data analytics features
Troubleshooting
If you encounter issues with the network startup:

Ensure all Docker containers are stopped: docker stop $(docker ps -q)
Remove all containers: docker rm $(docker ps -aq)
Remove all volumes: docker volume prune -f
Restart the network: ./start-minimal-network.sh


Acknowledgments
Hyperledger Fabric documentation and samples
ESP8266 and DHT11 sensor communities
The open-source community for blockchain and IoT technologies
