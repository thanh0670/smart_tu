const mongoose = require('mongoose');
const cardModel = require('../cardModel');
const connectDb = require('../dbConnect');

const clearDataExceptLast = async () => {
    try {
        await connectDb()
        const lastDocument = await cardModel.findOne().sort({ _id: -1 }).exec();
        if (lastDocument) {
            await cardModel.deleteMany({ _id: { $ne: lastDocument._id } });
            console.log('Cleared all data except the last document successfully');
        } else {
            console.log('No documents found in the collection');
        }
    }
    catch (error) {
        console.error('Error clearing data:', error);
        process.exit(1);
    }
    finally {

        await mongoose.disconnect();
    }
}

clearDataExceptLast();