<?php

    /**
    * Returns a stock by symbol (case-insensitively) else false if not
    */

    function lookup($symbol)
    {
        // reject symbols that start with ^
        if (preg_match("/^\^/", $symbol))
        {
            return false;
        }
        
        // reject synbols that contain commas
        if (preg_match("/,/", $symbol))
        {
            return false;
        }

        // headers for proxy servers
        $headers = [
            "Accept" => ""
        ]
    }
?>
