const PORT = process.env.PORT || 5000;
var express = require("express");
var cors = require("cors");
var morgan = require("morgan");
var http = require("http");
var path = require('path')

var app = express();

var server = http.createServer(app);

app.use(express.json());

app.use(cors({
    origin: true
}));

app.use(morgan('dev'));


app.post("/posttemp", (req, res, next) => {
    console.log("Readind :", req.body)
})


server.listen(PORT, () => {
    console.log("Server is Running:", PORT);
});