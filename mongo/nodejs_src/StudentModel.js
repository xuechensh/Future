const connection = require('./connection');
const StudentSchema = require('./StudentSchema');

const StudentModel = connection.model('student', StudentSchema);

module.exports = StudentModel;

require('./StudentMethod')
