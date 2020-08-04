<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>Seantinel</h2>
  <p>
    <span class="dht-labels">Temperature</span>
    <span id="temperature"> -- </span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <span class="dht-labels">Humidity</span>
    <span id="humidity"> -- </span>
    <sup class="units">%</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
 fetch('http://192.168.4.1/temperature')
  .then(response => response.json())
  .then(data =>{
    document.getElementById("temperature").innerHTML = data
    })
}, 2000 ) ;

setInterval(function ( ) {
 fetch('http://192.168.4.1/humidity')
  .then(response => response.json())
  .then(data =>{
    document.getElementById("humidity").innerHTML = data
    })
}, 2000 ) ;
</script>
</html>