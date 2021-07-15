const {Pool} = require('pg');
const pool = new Pool({
    host:'localhost',
    user:'postgres',
    password:'',
    database:'firstapi'
});

const getContact = (rq,res) =>{
    res.render('contact',{title:'Contacto'});
}
const getIndex = (rq,res) =>{
    res.render('index',{title:'Papelerias Blue Silver'});
}
const getUsers = async (rq,res) =>{
    const response  = await pool.query('SELECT * FROM users');
    console.log(response.rows);
    res.send('users');
}
module.exports = {
    getIndex,
    getContact,
    getUsers
}