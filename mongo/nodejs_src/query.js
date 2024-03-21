const connection = require('./connection')
const StudentModel = require('./StudentModel');

async function find() {
  const docs = await StudentModel.findStudent('liming');
  if (docs.length === 0) {
    let studentDoc = new StudentModel({
      name: 'liming',
      age: 18,
    });
    const rlt = await studentDoc.save();
    console.log(rlt);
  } else {
    docs.forEach((doc, index) => {console.log(doc, 'index: ', index)});
  }

  let rlt = await StudentModel.updateMany({name: 'liming', age: 18}, {age: 30});
  console.log(rlt);
}

find().then(() => {
  connection.close();
})
