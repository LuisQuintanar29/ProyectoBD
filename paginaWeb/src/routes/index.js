const express = require('express');
const router = express.Router();

const {getIndex, getContact, getUser, getShop} = require('../controllers/index.controller')

router.get('/',getIndex);
router.get('/user',getUser);
router.get('/contact',getContact);
router.get('/shop',getShop);

module.exports = router;    