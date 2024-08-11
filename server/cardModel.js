const mongoose = require("mongoose");

const cardSchema = mongoose.Schema({
    UID: {
        type: String,
        require: [true, "WHAT IS YOUR UID?"],
    },
    access: {
        type: String,
        require: [true, "DO YOU HAVE A ACCESS?"],
    },
    time: {
        type: String,
        require: [true, "WHAT IS YOUR Time?"],
    },
}, {
    timestamps: true,
})

module.exports = mongoose.model("card", cardSchema);