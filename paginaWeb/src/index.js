const express = require('express');
const app = express();
const path = require('path');

//Settings
app.set('port',3000);
app.set('views',path.join(__dirname,'views'));
app.set('view engine','ejs');

//Midlewares
app.use(require('./routes/'));

//Routes

//Static files
app.use(express.static(path.join(__dirname,'public')));

//Listening the server
app.listen(app.get('port'),() => {
    console.log('Server on port',app.get('port'));
})