const mongoose = require("mongoose");

const connectDb = async () => {
    try {
        const connect = await mongoose.connect("mongodb+srv://thanhdv06708:2Y5IQ2IC@smarttu.uvexd.mongodb.net/?retryWrites=true&w=majority&appName=smarttu");
        console.log("Database connected: ", connect.connection.host, connect.connection.name);
    } catch (err) {
        console.log(err);
        process.exit(1);
    }
}

module.exports = connectDb;