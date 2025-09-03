package main

import (
	"encoding/json"
	"fmt"
	"time"

	"github.com/hyperledger/fabric-contract-api-go/contractapi"
)

type SmartContract struct {
	contractapi.Contract
}

type SensorData struct {
	DeviceID    string  `json:"deviceID"`
	Temperature float64 `json:"temperature"`
	Humidity    float64 `json:"humidity"`
	Timestamp   string  `json:"timestamp"`
}

// Add new sensor data
func (s *SmartContract) AddSensorData(ctx contractapi.TransactionContextInterface, deviceID string, temperature float64, humidity float64) error {
	data := SensorData{
		DeviceID:    deviceID,
		Temperature: temperature,
		Humidity:    humidity,
		Timestamp:   time.Now().Format(time.RFC3339),
	}
	dataJSON, err := json.Marshal(data)
	if err != nil {
		return err
	}
	return ctx.GetStub().PutState(deviceID+"_"+data.Timestamp, dataJSON)
}

// Query single record
func (s *SmartContract) QuerySensorData(ctx contractapi.TransactionContextInterface, key string) (*SensorData, error) {
	dataJSON, err := ctx.GetStub().GetState(key)
	if err != nil {
		return nil, fmt.Errorf("failed to read from world state: %v", err)
	}
	if dataJSON == nil {
		return nil, fmt.Errorf("no data found for key: %s", key)
	}

	var data SensorData
	err = json.Unmarshal(dataJSON, &data)
	if err != nil {
		return nil, err
	}
	return &data, nil
}

func main() {
	chaincode, err := contractapi.NewChaincode(new(SmartContract))
	if err != nil {
		panic("Error creating iot chaincode: " + err.Error())
	}
	if err := chaincode.Start(); err != nil {
		panic("Error starting iot chaincode: " + err.Error())
	}
}
