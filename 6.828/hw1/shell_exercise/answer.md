# Questions

1. Give a series of UNIX commands that will produce a "long" listing of the smallest 5 files in the /etc directory whose name contains the string ".conf", sorted by increasing file size

   ```shell
    ls -lrS /etc| grep ".conf"| head -n 5
    ```

2. Give a series of UNIX commands that will produce the number of words in the file /usr/share/dict/words which do not contain any of the letters a, e, i, o, or u (upper and lower case)

    ```shell
    grep -cv '[aeiouAEIOU]' /usr/share/dict/words
    ```
