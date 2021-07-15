document.getElementById("register").addEventListener("click",register);
document.getElementById("login").addEventListener("click",IniciarSesion);
window.addEventListener("resize",PageWidth);    

var backbox_login = document.querySelector(".backbox_login");
var backbox_register = document.querySelector(".backbox_register");
var formulario = document.querySelector(".container_forms");
var formulario_login = document.querySelector(".form__login");
var formulario_register = document.querySelector(".form__register");

function PageWidth(){
    if(window.innerWidth > 850){
        backbox_login.style.display = "block";
        backbox_register.style.display = "block";
    }
    else{
        backbox_register.style.display = "block";
        backbox_register.style.opacity = "1";
        backbox_login.style.display = "none";
        formulario_login.style.display = "block";
        formulario_register.style.display = "none";
        formulario.style.left = "0";
    }
}
PageWidth();
function IniciarSesion(){ 
    if(window.innerWidth > 850 ){
        formulario_register.style.display = "none";
        formulario.style.left = "10px";
        formulario_login.style.display = "block";
        backbox_register.style.opacity = "1";
        backbox_login.style.opacity = "0";
    }
    else{
        formulario_register.style.display = "none";
        formulario.style.left = "0px";
        formulario_login.style.display = "block";
        backbox_register.style.display = "block";
        backbox_login.style.display = "none";
    }
}
function register(){ 
    if(window.innerWidth > 850){
        formulario_register.style.display = "block";
        formulario.style.left = "410px";
        formulario_login.style.display = "none";
        backbox_register.style.opacity = "0";
        backbox_login.style.opacity = "1";
    }
    else{
        formulario_register.style.display = "block";
        formulario.style.left = "0px";
        formulario_login.style.display = "none";
        backbox_register.style.display = "none";
        backbox_login.style.display = "block";
        backbox_login.style.display = "1";
    }
    
}