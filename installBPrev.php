#!/usr/bin/php
<?php
require_once '/usr/share/php/PhpAmqpLib/autoload.php';
use PhpAmqpLib\Connection\AMQPStreamConnection;
use PhpAmqpLib\Message\AMQPMessage;
require_once ('connectBEProd.inc');

$data = array();
$data['type'] = "rollback";

$msg = json_encode($data);

$deploy_r = new DeployClient();
$response = $deploy_r->call($msg);
$result - json_decode($response, true);
echo "Got $result";

?>
