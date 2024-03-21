const mongoose = require('mongoose');
const conn = mongoose.createConnection(
    'mongodb://localhost:27017/test',
    {
        useNewUrlParser: true,
        useUnifiedTopology: true
    }
);

conn.on('open', () => {
    console.log('open...');
});

conn.on('error', (err) => {
    console.log('error: ', err);
});

module.exports = conn;
