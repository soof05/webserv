#!/Users/sel-ouaf/Desktop/webserv/cgi-bin/php-cgi
<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    header("Content-Type: text/plain");
    $name = $_POST['name'] ?? 'hola';
    $email = $_POST['email'] ?? 'hola';
    echo "Name: $name\n";
    echo "Email: $email\n";
} else {
    header("Content-Type: text/html");
    ?>
    <form action="form.php" method="POST">
        <label for="name">Name:</label>
        <input type="text" name="name" id="name" required><br>
        <label for="email">Email:</label>
        <input type="email" name="email" id="email" required><br>
        <input name="submit" type="submit" value="Submit">
    </form>
    <?php
}
?>
