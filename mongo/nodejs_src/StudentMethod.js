const applyStatics = require('mongoose/lib/helpers/model/applyStatics');
const StudentModel = require('./StudentModel');
const StudentSchema = require('./StudentSchema');

StudentSchema.statics = {
  async findStudent(studentName) {
    return await this.find({name: studentName}, {name: 1, age: 1, _id: 0});
  }
}

applyStatics(StudentModel, StudentSchema);
