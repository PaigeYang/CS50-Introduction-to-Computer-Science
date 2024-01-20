<script>
            // TODO: Add code to check answers to questions
    document.addEventListener('DOMContentLoaded', function() {
        document.querySelector('form').addEventListener('submit', function (event) {
            let part2 = document.quertSelector('#part2').value;
            let display = document.querySelector('p');
            if (part2 == "Switzerland") {
                display.innerHTML = 'Correct'
            }
            else {
                name.innerHTML = 'Incorrect'
            }
        });
            });
</script>