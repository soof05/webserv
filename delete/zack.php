<!DOCTYPE html>

<html>

<head>

    <title>PHP Form Example</title>

</head>

<body>

    <h1>PHP Form Example</h1>

    <form method="post" action="zack.php">

        <label for="name">Name:</label>

        <input type="text" name="name" id="name"><br><br>

        <label for="email">Email:</label>

        <input type="email" name="email" id="email"><br><br>

        <input type="submit" name="submit" value="Submit">

    </form>

    <form action="pp.php" method="post" enctype="multipart/form-data">

        <input type="file" name="fileToUpload" multiple>

        <input type="submit" value="Upload">

    </form>

    <?php

        if ($_SERVER["REQUEST_METHOD"] == "POST") {
        
            $name = $_POST["name"];

            $email = $_POST["email"];

            echo "<h2>Your Input:</h2>";

            echo "Name: " . $name . "<br>";

            echo "Email: " . $email;

        }

    ?>

</body>

</html>
