
<?php
$hostname = gethostname ();
$uname = php_uname ();
$chrony_tracking=shell_exec("chronyc tracking"); 

echo "<center><h1>Welcome to $hostname</h1>";
echo "$uname<br>";
echo "<big>";

echo "<hr>";
echo "<table  border='1'>";
echo "<td><pre>".$chrony_tracking."</pre>";
echo "</table>";

echo "<hr>LINK";
echo "<table  border='1'>";
echo "<td><a href=data/>DATA FS</a>";
echo "<td><a href=data/logs/homerd/homer.txt>homer.txt</a>";
echo "<td><a href=data/logs/homerd/device.txt>device.txt</a>";
echo "<td><a href=data/logs/homerd/sensors.txt>sensors.txt</a>";
echo "</table>";

echo "<hr>NEXT TODO";
echo "<table  border='1'>";
echo "<td>HW<td><tr>";
echo "<td>HW<td><tr>";
echo "<td>SW FIX<td>Core empty submenu<tr>";
echo "<td>SW FIX<td>Reset MS on EV_KEY Key=106 (BUTTON_RIGHT) PRESSED ms=100 isLong=N<tr>";
echo "</table>";
echo "</center>";
?>