const express = require('express')
const app = express()
const port = 8000
const cors = require('cors')
const bodyParser = require('body-parser')
const { DateTime } = require('luxon');
const connectDb = require('./dbConnect');
const cardModel = require('./cardModel');


app.use(cors());
app.use(bodyParser.json())

let setUID = "";
let access = "SWIPE YOUR CARD";
let setKey = "";

connectDb();

app.get('/getCard', (req, res) => {
    const card = {
        UID: setUID,
        access: access,
        key: setKey
    };
    res.send(card);
})

app.post('/setCard', async (req, res) => {
    const { UID } = req.body
    const time = String(DateTime.now().setZone('Asia/Ho_Chi_Minh').toFormat('yyyy-MM-dd HH:mm:ss'));
    setUID = UID;
    access = "ACCESS GRANTED";
    setKey = "access";

    console.log("Access Granted:", setUID);
    console.log(access);
    console.log(time);




    const card = await cardModel.create({
        UID: setUID,
        access: access,
        time: time,
    });


    res.send(UID)
})

app.post('/setDeniedCard', async (req, res) => {
    const { UID } = req.body;
    const time = String(DateTime.now().setZone('Asia/Ho_Chi_Minh').toFormat('yyyy-MM-dd HH:mm:ss'));
    setUID = "DENIED"
    access = "ACCESS DENIED";
    setKey = "denied";
    console.log("ACCESS DENIED:", UID);
    console.log(access);
    console.log(time);



    const card = await cardModel.create({
        UID: setUID,
        access: access,
        time: time,
    });


    res.send("ACCESS DENIED")
})

app.listen(port, () => {
    console.log(`Example app listening on port ${port}`)
})