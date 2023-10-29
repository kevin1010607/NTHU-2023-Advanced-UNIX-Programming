# Advanced-UNIX-Programming_Student

## Track original file
```bash
git clone https://github.com/kevin1010607/Advanced-UNIX-Programming_Student.git
cd Advanced-UNIX-Programming_Student

# Fetch and merge new file from ta's repo
git remote add tas https://github.com/Jia-WeiFang/Advanced-UNIX-Programming_Student.git
git fetch tas
git merge tas/main --allow-unrelated-histories
```
