const express = require('express');
const router = express.Router();

const {getIndex, getContact, getUsers} = require('../controllers/index.controller')

router.get('/',getIndex);
router.get('/users',getUsers);

router.get('/contact',getContact);

module.exports = router;    