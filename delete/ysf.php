#!/usr/bin/php
<?php
if ($_SERVER['REQUEST_METHOD'] === 'GET') {
    header("Content-Type: text/plain");
    $name = $_GET['name'] ?? '';
    $email = $_GET['email'] ?? '';
    echo "Name: $name\n";
    echo "Email: $email\n";
} else {
    header("Content-Type: text/html");
    ?>
    <form method="GET">
        <label for="name">Name:</label>
        <input type="text" name="name" id="name" required><br>
        <label for="email">Email:</label>
        <input type="email" name="email" id="email" required><br>
        <input type="submit" value="Submit">
    </form>
    <?php
}
?>
