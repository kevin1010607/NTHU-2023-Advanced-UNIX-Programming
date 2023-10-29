# NTHU-2023-Advanced-UNIX-Programming

## Track original file
```bash
git clone https://github.com/kevin1010607/NTHU-2023-Advanced-UNIX-Programming.git
cd NTHU-2023-Advanced-UNIX-Programming

# Fetch and merge new file from ta's repo
git remote add tas https://github.com/Jia-WeiFang/Advanced-UNIX-Programming_Student.git
git fetch tas
git merge tas/main --allow-unrelated-histories
```
