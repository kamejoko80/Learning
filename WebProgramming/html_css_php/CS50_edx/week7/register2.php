<?php

    require("PHPMailer/class.phpmailer.php");

    if (!empty($_POST["name"]) && !empty($_POST["gender"]) && !empty($_POST["dorm"]))
    {
        $mial = new PHPMailer();
        
        $mial->IsSMTP();
        $mial->Host = "smtp.youhost.com";
        
        $mial->SetFrom("youremail@youhost.com");
        $mial->AddAddress("youremail@youhost.com");
        $mial->Subject = "registration";

        $mial->Body = 
            "This person just registered:\n\n" .
            "Name: " . $_POST["name"] . "\n" .
            "Captain: " . $_POST["captain"] . "\n" .
            "Gender: " . $_POST["gender"] . "\n" .
            "Dorm: " . $_POST["dorm"];
        if ($mial->Send() == false)
        {
            die($mial->ErrInfo);
        }
        else
        {
            Your are registered!
        }
    }
    else
    {
        You must provide your name, gender and dorm! Go <a href="froshims.php">back</a>
    }

?>


