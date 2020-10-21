import React from "react";
//import logo from "./logo.svg";
import "./App.css";
import soccer from "./soccer.jpg";
import hx5x from "./hx5x.jpg";
import hewitt from "./hewitt.jpg";
import terminator from "./terminator.jpg";
import mission from "./mission.jpg";
import { BrowserRouter as Router, Switch, Route, Link } from "react-router-dom";
import Form1 from "./Form1";
import Form2 from "./Form2";

function App() {
  //window.LOG_LEVEL = "DEBUG";
  return (
    <Router>
      <div>
        <Link className="xlink" to="/">
          <h1>Home (click to go Home)</h1>
        </Link>
        <Link className="xlink1" to="/form1">
          <h1>Form1 (click to go Form1)</h1>
        </Link>
        <Link className="xlink2" to="/form2">
          <h1>Form2 (click to go Form2)</h1>
        </Link>

        <Switch>
          <Route path="/form2">
            <Form2 />
          </Route>
          <Route path="/form1">
            <Form1 />
          </Route>
          <Route path="/">
            <Home />
          </Route>
        </Switch>
      </div>
    </Router>
  );
}

function Home() {
  return (
    <div className="App">
      <h1>Home</h1>
      <img className="App-logo" src={soccer} alt="logo" />
      <img className="App-logo" src={hx5x} alt="logo" />
      <img className="App-logo" src={hewitt} alt="logo" />
      <img className="App-logo" src={terminator} alt="logo" />
      <img className="App-logo" src={mission} alt="logo" />
    </div>
  );
}

export default App;
