import React from "react";
import ReactDOM from "react-dom";
import { Redirect } from "react-router-dom";

let AWS = require("aws-sdk");
AWS.config.region = "us-east-1";
AWS.config.credentials = new AWS.CognitoIdentityCredentials({
  IdentityPoolId: "us-east-1:02287482-d79e-41df-a06d-53e7805cfc30",
  // Federated Identities - idpool
});
let lambda = new AWS.Lambda();

let count = 0;
let xinterval;

const xtime = 5000;
const device1 = "nodemcu32s";
const device2 = "nodemcu32s1";
const dispdata1 = "dispdata1";
const dispdata2 = "dispdata2";

//window.LOG_LEVEL = "DEBUG";

// *************************************************************
class Form2 extends React.Component {
  constructor(props) {
    super(props);
    this.state = { OnOff: "OFF" };

    this.handleButtonClick = this.handleButtonClick.bind(this);
  }

  handleButtonClick() {
    if (this.state.OnOff === "ON") {
      console.log("ON to OFF ", xinterval);
      this.setState({ OnOff: "OFF" });
      clearInterval(xinterval);
    } else {
      console.log("OFF to ON ", xinterval);
      this.setState({ OnOff: "ON" });
      xinterval = setInterval(() => {
        count = count + 1;
        this.execLambda(device1, dispdata1);
        this.execLambda(device2, dispdata2);
      }, xtime);
    }
  }

  execLambda(xdevice, xdispdata) {
    lambda.invoke(
      {
        FunctionName: "lamiot",
        InvocationType: "RequestResponse",
        Payload: JSON.stringify({ key1: xdevice }),
      },
      function (err, data) {
        //console.log("function - data >>>", data);
        if (err) {
          console.log("error", err);
        } else {
          let dataIoT =
            JSON.stringify(count) +
            " - " +
            JSON.stringify(JSON.parse(data.Payload));
          //console.log("dataIoT: ",dataIoT);
          ReactDOM.render(
            <h3>
              {new Date().toLocaleTimeString()} Count: {dataIoT}
            </h3>,
            document.getElementById(xdispdata)
          );
        }
      }
    );
  }

  DisplayDate() {
    ReactDOM.render(
      <a>{new Date().toLocaleTimeString()}</a>,
      document.getElementById("dispdate")
    );
  }

  render() {
    return (
      <div>
         <dat id="dispdate" style={{
            backgroundColor: "purple",
            fontSize: 30,
            fontWeight: "bold",
            color: "pink",
            width: "fit-content",
            height: "fit-content",
            display: "flex"
          
          }}> </dat>
        
        <label
          style={{
            backgroundColor: "lightgreen",
            fontSize: 30,
            fontWeight: "bold",
            marginLeft: "20px",
            marginRight: "20px",
            color: "black",
          }}
        >
          Data collection is {this.state.OnOff}
        </label>

        <button
          onClick={this.handleButtonClick}
          style={{
            backgroundColor: "red",
            fontSize: 30,
            color: "white",
            fontWeight: "bold",
            marginLeft: "20px",
            marginRight: "20px",
          }}
        >
          Click here to turn ON / OFF data collection
        </button>
       
        <br></br>
        <div id="dispdata1"></div>
        <div id="dispdata2"></div>
      </div>
    );
  }
  intervalIdX = setInterval(this.DisplayDate, 1000);
}

export default Form2;
