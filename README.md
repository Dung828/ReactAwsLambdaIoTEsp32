# ReactAwsLambdaIoTEsp32    21/10/2020 
This project is to build a web page which collects data from AWS IoT Shadow device ESP32 DEVKIT V1 ( ESP-WROOM-32 or NodeMCU-32S ). 
IoT device sends data to AWS IoT Shadow document with MQTT.
AWS Lambda lamiot.js collects IoT device Shadow document.
The Web page is designed with REACT and Javascript / Lambda function .
Files :  lamiot.js ( AWS Lambda function ) ,  nodeMcu32s_iotB.ino / secrets.h ( ESP32 code with MQTT ) , React code ( index.js , App.js , App.css , Form1.js , Form2.js ).  
AWS Lambda function - role with policy IoTDataAccess 
AWS Cognito - Federated Identities - create a pool id.
REACT - include aws-sdk , router / link .
