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
const getUser = (rq,res) =>{
    res.render('user',{title:'Usuario'});
}
const getShop = (rq,res) =>{
    res.render('shop',{title:'TIenda'});
}
module.exports = {
    getIndex,
    getContact,
    getUser,
    getShop
}