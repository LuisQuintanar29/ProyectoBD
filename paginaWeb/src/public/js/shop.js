const {Pool} = require('pg');

const config = {
    user: 'admin',
    host: 'localhost',
    password: 'admin',
    database: 'papeleria'
};

const pool = new Pool(config);

const getInvetario = async() =>{
    try{
        const res = await pool.query("SELECT * FROM view_compra_prod");
        console.log(res.rows);
    }catch(e){
        console.log(e);
    }
};

getInvetario();