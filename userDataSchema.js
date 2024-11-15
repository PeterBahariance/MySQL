// defines our userData object so that MongoDB knows what our notes object is and what data it holds
var mongoose = require("mongoose")
var Schema = mongoose.Schema
 
var userData = new Schema({

    email: String,
    userName: String,
    password: String,
    birthDate: String

})

// what this does is create the data object that is of type userData schema (defined above)
const Data = mongoose.model("Data", userData)

// sends our defined Schema to the server.js file   
module.exports = Data