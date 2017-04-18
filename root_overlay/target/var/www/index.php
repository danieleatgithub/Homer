
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
echo "<td><a href=data/log/homerd/homerd.txt>homerd.txt</a>";
echo "<td><a href=data/log/homerd/hdevice.txt>hdevice.txt</a>";
echo "<td><a href=data/log/homerd/sensors.txt>sensors.txt</a>";
echo "</table>";

echo "<hr>NEXT TODO";
echo "<table  border='1'>";
echo "<td>HW<td>RTC Battery<tr>";
echo "<td>HW FIX<td>Display light flickering<tr>";
echo "<td>SW<td>Sensor update log rolling month (log4cplus /etc/default/homerd.properties)<tr>";
echo "<td>SW FIX<td>Reset MS on EV_KEY Key=106 (BUTTON_RIGHT) PRESSED ms=100 isLong=N<tr>";
echo "</table>";
echo "</center>";
?>