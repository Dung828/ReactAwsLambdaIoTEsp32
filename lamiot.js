"use strict";
let AWS = require('aws-sdk');

let config = {
  "thingName": 'nodemcu32s',
  "endpointAddress": "atzpfe6u4qxeh-ats.iot.us-east-1.amazonaws.com"
}
let iotdata = new AWS.IotData({ endpoint: config.endpointAddress });

exports.handler = function(event, context) {
  //console.log("Dung-event: ", event);
  //console.log("Dung-event.key1: ", event.key1);
  //console.log("Dung-JSON.stringify-event: ", JSON.stringify(event));

  if (event.key1 == "nodemcu32s" || event.key1 == "nodemcu32s1") {
    //console.log('Dung-key1');
    config.thingName = event.key1;

    iotdata.getThingShadow({
      thingName: config.thingName
    }, function(err, data) {
      //console.log("Dung-err: >>> ", err);
      //console.log("Dung-data: >>> ", data);
      if (err) {
        context.fail(err);
        //console.log("Dung-err: ", err);
      }
      else {
        //console.log("Dung-OK");
        let jsonPayload = JSON.parse(data.payload);
        
        let reported = jsonPayload.state.reported;
        
        let count = jsonPayload.state.reported.count;
        let lostConnection = jsonPayload.state.reported.lostConnection;
        let localTime = jsonPayload.state.reported.localTime;
        let time = jsonPayload.state.reported.time;
        let wifi_strength = jsonPayload.state.reported.wifi_strength;
        let device = jsonPayload.state.reported.device;

        let reponder = reported;
        //console.log("Dung-reponder: ", reponder);
        context.succeed(reponder);
        
        //let response = {
        //  statusCode: 200,
        //  body: JSON.stringify(reponder)
        //}
      }
    });
  }
};
