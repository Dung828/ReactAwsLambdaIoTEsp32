import React from "react";
//import ReactDOM from "react-dom";

class Form1 extends React.Component {
  constructor(props) {
    super(props);
    this.state = { xname: "", xpassword: "" };

    this.handleChange = this.handleChange.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);
  }

  handleChange(event) {
    this.setState({ [event.target.name]: event.target.value });
    console.log(
      event.target.name,
      "|||",
      event.target.type,
      "---",
      event.target.value
    );
  }

  handleSubmit(event) {
    alert(this.state.xname + "  " + this.state.xpassword);
    event.preventDefault();
  }

  render() {
    //console.log("Form1");
    return (
      <div id="form1">
        <form onSubmit={this.handleSubmit}>
          <h1>Form1</h1>
          <label>
            Name:
            <input
              type="text"
              name="xname"
              value={this.state.xname}
              onChange={this.handleChange}
            />
          </label>
          <label>
            Password:
            <input
              type="text"
              name="xpassword"
              value={this.state.xpassword}
              onChange={this.handleChange}
            />
          </label>
          <input type="submit" value="Submit" />
        </form>
      </div>
    );
  }
}
export default Form1;
