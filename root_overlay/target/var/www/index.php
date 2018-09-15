<?php
$hostname = gethostname ();
$uname = php_uname ();
$chrony_tracking=shell_exec("chronyc tracking"); 
$date=shell_exec("date"); 
$buildroot=shell_exec('source /etc/os-release; echo $PRETTY_NAME'); 
$homer=shell_exec("cat /etc/homer.version"); 

echo "<center><h1>Welcome to $hostname</h1>";
echo "$uname<br>";
echo "<big>";

echo "<hr>";
echo "<table  border='1'>";
echo "<td>Homer: ".$homer."</td><tr>";
echo "<td>Buildroot: ".$buildroot."</td><tr>";
echo "<td><pre>".$chrony_tracking."Local Date      : ".$date."</pre>";
echo "</table>";

echo "<hr>LINK";
echo "<table  border='1'>";
echo "<td><a href=/data/>DATA FS</a>";
echo "<td><a href=/data/logs/homer.txt>homer.txt</a>";
echo "<td><a href=/data/logs/devices.txt>devices.txt</a>";
echo "<td><a href=/data/logs/sensors.txt>sensors.txt</a>";
echo "</table>";

echo "<hr>NEXT TODO";
echo "<table  border='1'>";
echo "<td>HW<td><tr>";
echo "<td>HW<td><tr>";
echo "<td>SW FIX<td>Core empty submenu<tr>";
echo "<td>SW FIX<td>
<pre>
351045126 [3058152528] void homerio::KeyPanel::keyThreadReader() DEBUG homerd.device  - EV_KEY Key=108 (BUTTON_DOWN) PRESSED ms=131 isLong=N
351045127 [3058152528] void homerio::KeyPanel::keyThreadReader() DEBUG homerd.device  - EV_SYN (113) Key=108 (BUTTON_DOWN) PRESSED ms=131 isLong=N
351048059 [3058152528] void homerio::KeyPanel::keyThreadReader() DEBUG homerd.device  - EV_KEY Key=108 (BUTTON_DOWN) RELEASED ms=131 isLong=N
351048060 [3058152528] void homerio::KeyPanel::keyThreadReader() DEBUG homerd.device  - EV_SYN (114) Key=108 (BUTTON_DOWN) RELEASED ms=2933 isLong=Y
Reset MS on EV_KEY Key=..... PRESSED ....
</pre>
<tr>";
echo "</table>";
echo "</center>";
?>
