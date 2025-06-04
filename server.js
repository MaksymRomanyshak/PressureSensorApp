const express = require("express");
const bodyParser = require("body-parser");
const path = require("path");

const app = express();
const PORT = process.env.PORT || 3000;

let latestData = {}; // Store the latest ESP8266 data
let lastReceivedTime = Date.now();
const TIMEOUT_MS = 3000;

app.use(bodyParser.json());

// Serve the UI from the "public" folder
app.use(express.static(path.join(__dirname, "public")));

// Endpoint to receive ESP8266 data
app.post("/data", (req, res) => {
  latestData = req.body; // Store latest data
  lastReceivedTime = Date.now();
  console.log("Received data:", latestData);
  res.json({ message: "Data received successfully" });
});

// API to send latest data to frontend
app.get("/data", (req, res) => {
  res.json(latestData);
});

// Start server
app.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`);
});

setInterval(() => {
  if (Date.now() - lastReceivedTime > TIMEOUT_MS) {
    latestData = {}; // Або latestData.pressure = undefined;
  }
}, 1000);
