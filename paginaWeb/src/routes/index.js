const express = require('express');
const router = express.Router();


router.get('/',(rq,res) =>{
    res.render('index',{title:'Papelerias Blue Silver'});
})

router.get('/contact',(rq,res) =>{
    res.render('contact',{title:'Contact'});
})

module.exports = router;