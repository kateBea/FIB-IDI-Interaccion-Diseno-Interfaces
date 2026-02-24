var i=0
var nota;

//function passara (Cap, Sec)
//{
//	top.onsoc.passara (Cap);
//	top.explorador.passara(Cap, Sec);
//}

function MuestraFin(destination,dimx,dimy,ok){      
         var text = "height="+dimy+",width="+dimx+',scrollbars='+ok
         var newWindow = window.open(destination,"",text);
}

/*
function oWin(destination,a,dimx,dimy,b,c,d,ok){  
	 
         var text = "height="+dimy+",width="+dimx+',scrollbars='+ok
         var newWindow= window.open(destination,'WIN'+i,text);
         i=i+1
}
*/

function oWin(plana,type,tam1,tam2,mbar,locat,rsize,stat){
 var resWin;
      var w;
      var h;
      var nom;
      var cadena;

 if (type == "*")
            nom= getWinNm();
 else nom=type;

 if ((tam1 == "*") || (tam2 == "*")) {
  w = 600;
  h = 400;
 }
 else {
     w=tam1;
     h=tam2;
 }
   
  cadena = 'width='+w+',height='+h+',menubar=' + mbar + ',location=' + locat + ',resizable=' + rsize + ',scrollbars=yes,status=' + stat;
  resWin = window.open(trobarURLabs(plana),nom,cadena); 

  if (NS) {
	resWin.focus();
  }
}


function glossari(nom)
{
	var newWindow = MuestraFin("../glossari/"+nom,400,300,"yes");
}

var prem;
prem = new String(location);

var NS = (navigator.appName == "Netscape") ? 1 : 0;
var IE = (navigator.appName == "Microsoft Internet Explorer") ? 1 : 0; 
var ver4 = (navigator.appVersion.indexOf('4.',0) == 0) ? 1 : 0;


function trobarURLabs(relativa){
  if(relativa.indexOf('http://',0)==0) return relativa;
  if(relativa.indexOf('ftp://',0)==0) return relativa;
  
//if(relativa.indexOf('mailto:',0)==0) return relativa;

  var n;
  var quants;
  var URLabs;

  quants = 0;
  URLabs = prem;
  
  if(relativa.indexOf('/',0)==0){
	if((n=URLabs.lastIndexOf('/web/'))==-1)
		n=URLabs.lastIndexOf('\\web\\');
	URLabs=URLabs.substring(0,n);
  }

  else
  {
	n = relativa.indexOf('../',0);
	if(n != -1){
    		while(n!=-1){
      		n = relativa.indexOf('../',n+3);
      		quants++;
    		}
    	relativa = relativa.substring((quants*3),relativa.length);
  	} 
  
  	if(URLabs.indexOf('\\',0)==-1)
  	{
  	  for(var i=0; i<quants+1;i++){
  	    URLabs = URLabs.substring(0,URLabs.length-1);
  	    URLabs = URLabs.substring(0,URLabs.lastIndexOf('/')+1);
  	  }
  	}
  	else{
  	 for(var i=0; i<quants+1;i++){
  	    URLabs = URLabs.substring(0,URLabs.length-1);
  	    URLabs = URLabs.substring(0,URLabs.lastIndexOf('\\')+1);
  	  }
  	}
  }
  URLabs +=relativa;
  return URLabs;
}     
    
function docLoad(Url, target) {
var loc;
loc = target;
loc += '.location';
eval(loc + '=trobarURLabs(Url);');
}


function getIm(imatge)
{
	var im;
	im = new Image;
	im.src=trobarURLabs(imatge);
	return (im.src);
}

function mouseOver(banner) {
 self.status=banner;
 }

function mouseOut() {
 self.status="";
 }

function getWinNm()     {
        var now = new Date();
        var hr = new String(now.getHours());
        var mn = new String(now.getMinutes());
        var sc = new String(now.getSeconds());
        var winNm = hr + mn + sc;return winNm;
        }

function cache()
{
this.length=cache.arguments.length;
for(i=0;i<this.length;i++)
	{
	this[i+1]=new Image();
	this[i+1].src=cache.arguments[i];
	}
}

