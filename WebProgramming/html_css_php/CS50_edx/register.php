<!DOCTYPE html>
<?php

    if (empty($_POST["name"]) || empty($_POST["name"]) || empty($_POST["name"]))
    {
        header("Location: http://localhost/froshims.php");
        exit;
    }

?>

<html>
    <head>
        <title>Frosh IMs</title>
    </head>
    <body>
        <pre>
            Your are registered!
            <?php print_r($_POST); ?>
        </pre>
    </body>
</html>
