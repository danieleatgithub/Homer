
<?php
$hostname = gethostname ();
$uname = php_uname ();

echo "<center><h1>Welcome to $hostname</h1>";
echo "$uname<br>";
echo "<table border='1'><th>PIN</th><th>Usage</th><th>Dts</th>";
echo "<tr><td>PA01</td><td>1-Wire</td><td>Y</td></tr>";
echo "<tr><td>PA02</td><td>LCD Reset</td><td>Y</td></tr>";
echo "<tr><td>PA03</td><td>LCD Backlight/td><td>Y</td></tr>";
echo "</table></center>";

echo "<hr>";
echo "<center><big>";
echo "<table  border='1'>";
echo "<td><a href=data/>DATA FS</a>";
echo "<td><a href=data/logs/homerd.txt>homerd.txt</a>";
echo "<td><a href=data/logs/hdevice.txt>hdevice.txt</a>";
echo "";
echo "</table></center>";


?>
