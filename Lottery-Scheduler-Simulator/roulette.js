function addField(plusElement){

	let displayButton = document.querySelector("form button");

	// Stopping the function if the input field has no value.
	if(plusElement.previousElementSibling.value.trim() === ""){
		return false;
	}

	// creating the div container.
	let div = document.createElement("div");
	div.setAttribute("class", "field");

	// Creating the input element.
	let field = document.createElement("input");
	field.setAttribute("type", "number");
	field.setAttribute("name", "notes[]");
	field.setAttribute("min", "1")

	// Creating the plus span element.
	let plus = document.createElement("span");
	plus.setAttribute("onclick", "addField(this)");
	let plusText = document.createTextNode("+");
	plus.appendChild(plusText);

	// Creating the minus span element.
	let minus = document.createElement("span");
	minus.setAttribute("onclick", "removeField(this)");
	let minusText = document.createTextNode("-");
	minus.appendChild(minusText);

	// Adding the elements to the DOM.
	form.insertBefore(div, displayButton);
	div.appendChild(field);
	div.appendChild(plus);
	div.appendChild(minus);

	// Un hiding the minus sign.
	plusElement.nextElementSibling.style.display = "block"; // the minus sign
	// Hiding the plus sign.
	plusElement.style.display = "none"; // the plus sign
}

function removeField(minusElement){
   minusElement.parentElement.remove();
}

let form = document.forms[0];
form.addEventListener("submit", fetchTextNotes);
form.addEventListener("submit", spin);
function fetchTextNotes(event){
	// prevent the form to communicate with the server.
	event.preventDefault();

	// Fetch the values from the input fields.
	let data = new FormData(form);

	// Storing the values inside an array so we can handle them.
	// we don't want empty values.
	let notes = [];
	data.forEach( function(value){
		if(value !== ""){
			notes.push(value);
		}
	});//string to integer
    function toNumber(value) {
		return Number(value);
	 }
	 
   
  //Console log
  var options = notes.map(toNumber);
  console.log(options);
  var startAngle = 0;
  //var arc = Math.PI / (options.length / 2);
  //var arc = []
  
  var spinTimeout = null;

  var spinArcStart = 10;
  var spinTime = 0;
  var spinTimeTotal = 0;

  var ctx;

  //split circle proportionally
  const sum = options.reduce((partialSum, a) => partialSum + a, 0);
  console.log(sum);
  const frac = (2 * Math.PI) / sum
  console.log(frac);
  var arc = []
  for(var i = 0; i < options.length; i++) {
    arc.push(options[i]*frac);
  console.log(arc)
  }



  function byte2Hex(n) {
    var nybHexString = "0123456789ABCDEF";
    return String(nybHexString.substr((n >> 4) & 0x0F,1)) + nybHexString.substr(n & 0x0F,1);
  }

  function RGB2Color(r,g,b) {
    return '#' + byte2Hex(r) + byte2Hex(g) + byte2Hex(b);
  }

  function getColor(item, maxitem) {
    var phase = 0;
    var center = 128;
    var width = 127;
    var frequency = Math.PI*2/maxitem;
    
    red   = Math.sin(frequency*item+2+phase) * width + center;
    green = Math.sin(frequency*item+0+phase) * width + center;
    blue  = Math.sin(frequency*item+4+phase) * width + center;
    
    return RGB2Color(red,green,blue);
  }

  function drawRouletteWheel() {
    var canvas = document.getElementById("canvas");
    if (canvas.getContext) {
      var outsideRadius = 200;
      var textRadius = 145;
      var insideRadius = 100;

      ctx = canvas.getContext("2d");
      ctx.clearRect(0,0,400,400);

      ctx.strokeStyle = "black";
      ctx.lineWidth = 2;

      ctx.font = 'bold 12px Helvetica, Arial';
      
      
      for(var i = 0; i < arc.length; i++) {
        var angle = startAngle + i * arc[i];
        ctx.fillStyle = getColor(i, arc.length);

        ctx.beginPath();
        ctx.arc(250, 250, outsideRadius, angle, angle + arc[i], false);
        ctx.arc(250, 250, insideRadius, angle + arc[i], angle, true);
        ctx.stroke();
        ctx.fill();

        ctx.save();
        ctx.shadowOffsetX = -1;
        ctx.shadowOffsetY = -1;
        ctx.shadowBlur    = 0;
        ctx.shadowColor   = "rgb(220,220,220)";
        ctx.fillStyle = "black";
        ctx.translate(250 + Math.cos(angle + arc[i] / 2) * textRadius, 
                      250 + Math.sin(angle + arc[i] / 2) * textRadius);
        ctx.rotate(angle + arc[i] / 2 + Math.PI / 2);
        var text = options[i];
        ctx.fillText(text, -ctx.measureText(text).width / 2, 0);
        ctx.restore();
        //console.log(arc[i])
      } 

      //Arrow
      ctx.fillStyle = "black";
      ctx.beginPath();
      ctx.moveTo(250 - 4, 250 - (outsideRadius + 5));
      ctx.lineTo(250 + 4, 250 - (outsideRadius + 5));
      ctx.lineTo(250 + 4, 250 - (outsideRadius - 5));
      ctx.lineTo(250 + 9, 250 - (outsideRadius - 5));
      ctx.lineTo(250 + 0, 250 - (outsideRadius - 13));
      ctx.lineTo(250 - 9, 250 - (outsideRadius - 5));
      ctx.lineTo(250 - 4, 250 - (outsideRadius - 5));
      ctx.lineTo(250 - 4, 250 - (outsideRadius + 5));
      ctx.fill();
    }
    
  }

  function spin() {
    spinAngleStart = Math.random() * 10 + 10;
    spinTime = 0;
    spinTimeTotal = Math.random() * 3 + 4 * 1000;
    rotateWheel();
  }

  function rotateWheel() {
    spinTime += 30;
    if(spinTime >= spinTimeTotal) {
      stopRotateWheel();
      return;
    }
    var spinAngle = spinAngleStart - easeOut(spinTime, 0, spinAngleStart, spinTimeTotal);
    startAngle += (spinAngle * Math.PI / 180);
    drawRouletteWheel();
    spinTimeout = setTimeout(rotateWheel, 30);
  }

  function stopRotateWheel() {
    clearTimeout(spinTimeout);
    //var degrees = startAngle * 180 / Math.PI + 90;
    // var arcd = [];
    // for(var i = 0; i < arc.length; i++) {
    //   arcd.push(arc[i] * 180 / Math.PI);
    // console.log(arcd)
    // }
    // var arcd = arc[i] * 180 / Math.PI;
    // var index = Math.floor((360 - degrees % 360) / arcd);
    // ctx.save();
    // ctx.font = 'bold 30px Helvetica, Arial';
    // var text = options[index]
    //ctx.fillText(text, 250 - ctx.measureText(text).width / 2, 250 + 10);
    //ctx.restore();
  }

  function easeOut(t, b, c, d) {
    var ts = (t/=d)*t;
    var tc = ts*t;
    return b+c*(tc + -3*ts + 3*t);
  }

  drawRouletteWheel();
  spin();
}