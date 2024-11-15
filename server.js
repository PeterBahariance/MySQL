// make sure to understand what these 2 lines of code actually do
const express = require('express')
const mongoose = require('mongoose')

// creates an express object to actually be used in our project
var app = express()


// No .js required after file name? 
var Data = require('./userDataSchema')


// connects our mongoose to mongoDB, which runs our local machine
// myDB is the folder where all our data will actually be stored
mongoose.connect("mongodb://localhost/myDB")


// Once mongose has made a connection, we are notified that it is opened
// We are also notified if it fails to open, and our function displays the error message for us 
mongoose.connection.once("open", function(){
    
    console.log("Connected to Data Base!")

}).on("error", function(error){

    console.log("Failed to connect:" + error)

})  


// CREATE A NOTE (sending something to the server, called POST request)
// req = request (information the iPhone is sending us.)
// Request object contains: the note, the title, and the date

// res = response (what we are sending back)
// Response object contains: everything in the note that is now stored 
app.post("/create", (req, res) => {


    var userData = new Data ({

        // must match the Schema for the note object in noteSchema.js
        // the following holds the information that the iPhone sends us 
        email: req.get("email"),
        userName: req.get("userName"),
        password: req.get("password"),
        birthDate: req.get("birthDate"),

        // has to match in postman as well!
    
    })

    // saves it to our data base
    // => is the same thing as writing function() 
    userData.save().then(() =>   {
        // if isNew is true, then that means that the data is only in the server, and not the data base!
        if(userData.isNew == false){

            console.log("Saved data!")
            res.send("Saved data!")
        }
        else{

            console.log("Failed to save data")
        }
    })

    
})

// 192.168.1.166 is the IP address under the network PeterWifi in the apartment,
// Meaning that any device connected to this network, like my iphone or physical laptop
// will be able to access this server

// You can access server at:
// http://192.168.1.166/create
var server = app.listen(8081, "192.168.1.166", () => {

    console.log("Server is running!")

})

// FETCH ALL NOTES (getting something from the server, called GET request)
app.get('/fetch', (req, res) => {
    
    // {} gets every bit of data in the data base
    Data.find({}).then((DBitems) =>  {
        res.send(DBitems)
    })


})

// DELETE A NOTE (sending something to the server, called POST request)

// THIS WAS DIFFERENT FROM VIDEO, SINCE VIDEOS WAS OUT DATED, OBTAINED FROM STACK OVERFLOW!!!
app.post('/delete', async (req,res) => {
    try {
      await  Data.findOneAndDelete({ _id: req.get("id") })
      res.send("Deleted!!")
      console.log("Deleted!!")
    } catch(error) {   
      console.log("Failed" + error)
    }     
  })

// UPDATE A NOTE (sending something to the server, called POST request)
// THIS WAS DIFFERENT FROM VIDEO, SINCE VIDEOS WAS OUT DATED, OBTAINED FROM CHATGPT!!!

app.post('/update', async (req, res) => {
    try {
        // Perform the update operation
        const result = await Data.findOneAndUpdate(
            { _id: req.get("id") }, // Find document by _id
            {
                email: req.get("email"),
                userName: req.get("userName"),
                password: req.get("password"),
                birthDate: req.get("birthDate")
            },
            { new: true } // Return the updated document
        );

        // Check if the document was found and updated
        if (!result) {
            return res.status(404).send("No document found to update");
        }

        res.send("Updated!");
        console.log("Updated!");
    } catch (error) {
        console.log("Failed to update: " + error);
    }
});
