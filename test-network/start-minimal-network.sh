#!/bin/bash

# Create directories for artifacts
mkdir -p system-genesis-block channel-artifacts

# Bring up the network
docker-compose -f docker-compose-minimal.yml up -d

# Wait for the network to start
sleep 10

# Generate system channel genesis block
docker exec -e "FABRIC_CFG_PATH=/etc/hyperledger/configtx" -e "PATH=/etc/hyperledger/bin:$PATH" orderer.example.com configtxgen -profile TwoOrgsOrdererGenesis -channelID system-channel -outputBlock ./system-genesis-block/genesis.block

# Restart orderer to use the genesis block
docker-compose -f docker-compose-minimal.yml restart orderer.example.com

# Wait for orderer to restart
sleep 5

# Generate channel genesis block
docker exec -e "FABRIC_CFG_PATH=/etc/hyperledger/configtx" -e "PATH=/etc/hyperledger/bin:$PATH" peer0.org1.example.com configtxgen -profile TwoOrgsChannel -outputCreateChannelTx ./channel-artifacts/mychannel.tx -channelID mychannel

# Create the channel
docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" -e "CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/fabric/msp" peer0.org1.example.com peer channel create -c mychannel -f ./channel-artifacts/mychannel.tx -o localhost:7050 --outputBlock ./channel-artifacts/mychannel.block

# Join peer to the channel
docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" -e "CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/fabric/msp" peer0.org1.example.com peer channel join -b ./channel-artifacts/mychannel.block

echo "Network started successfully"
