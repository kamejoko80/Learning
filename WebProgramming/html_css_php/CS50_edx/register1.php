<!DOCTYPE html>

<html>
    <head>
        <title>Frosh IMs</title>
    </head>
    <body>
        <?php if (empty($_POST["name"]) || empty($_POST["gender"]) || empty($_POST["dorm"])): ?>
            You must provide your name, gender and dorm! Go <a href="froshims.php">back</a>
        <?php else: ?>
            Your are registered!
        <?php endif ?>
    </body>
</html>
