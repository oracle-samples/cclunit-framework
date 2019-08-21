# Troubleshooting

This documentation will help you to troubleshoot docker related issues while running your ccl unit test with "container" environment.

- [Troubleshooting](#troubleshooting)
    - [Docker Container](#docker-container)
    - [Timeout errors](#timeout-errors)
    - [Failed to start containers: ccloracle](#failed-to-start-containers-ccloracle)
    
### Docker Container
If you are getting the following error while executing the ccl unit test in Gaia, it means that you are using a wrong container in docker.
```sh
"error: 'Error: Command failed: docker pull docker-snapshot.cernerrepos.net/mpages/ccltesting \nimage operating system \"linux\" cannot 
be used on this platform\n' stdout = 'Using default tag: latest\nlatest: Pulling from mpages/ccltesting\n' stderr = 'image operating 
system \"linux\" cannot be used on this platform\n'"
```
To resolve this issue, you need to change the container from Windows to Linux.
1. Go to the docker settings and then click on the "Switch to Linux containers... " option.
2. The docker will switch the container and then it will automatically restart itself.
3. Once docker starts running, run your unit test again.

### Timeout Errors
If you are getting the following errors while executing the ccl unit test in Gaia, then it means that you might not be accessing the application within Cerner's Network.
```sh
"error: 'Error: Command failed: docker pull docker-snapshot.cernerrepos.net/mpages/ccltesting \nError response from daemon: Get 
https://docker-snapshot.cernerrepos.net/v2/: net/http: request canceled while waiting for connection (Client.Timeout exceeded while 
awaiting headers)\n' stdout = 'Using default tag: latest\n' stderr = 'Error response from daemon: Get https://docker-
snapshot.cernerrepos.net/v2/: net/http: request canceled while waiting for connection (Client.Timeout exceeded while awaiting 
headers)\n'"
```
To resolve this error, try fetching the docker images through Cerner network as they are available only for the internal network.

### Failed to start containers: ccloracle

If you encounter errors such as mentioned below, it means you have duplicate networks on which docker is running.
```sh
'Error response from daemon: network cclnetwork is ambiguous (2 matches found on name)\nError: failed to start containers: ccloracle\n'"
```
To resolve this issue, you need to remove the additional network and try running the test cases again.
1. Go to PowerShell and run the following command
```sh
docker network ls
```
2. If you find you have two network id with a same name then, you need to remove them to resolve this issue.
3. Before removing the network id, stop docker and then proceed further.
4. To remove the network id, use the following command in powershell
```sh
docker network rm Network Id
```
5. Restart docker and execute your test cases.

Sources:
* https://docs.docker.com/engine/reference/commandline/network_rm/
* https://github.com/Microsoft/DockerTools/issues/89
    
