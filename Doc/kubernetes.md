# Kubernetes kubectl Cheatsheet
## Installation

To install kubectl on Linux, you can use the following commands:

```bash

# Add the Kubernetes signing key
curl -s https://packages.cloud.google.com/apt/doc/apt-key.gpg | sudo apt-key add -

# Add the Kubernetes repository
echo "deb https://apt.kubernetes.io/ kubernetes-xenial main" | sudo tee /etc/apt/sources.list.d/kubernetes.list

# Install kubectl
sudo apt-get update && sudo apt-get install -y kubectl

```

### Basic commands

```
    kubectl get pods: list all running pods
    kubectl get services: list all services
    kubectl get deployments: list all deployments
    kubectl describe pod <pod-name>: show details about a specific pod
    kubectl logs <pod-name>: show logs for a specific pod
    kubectl exec -it <pod-name> /bin/bash: open a shell in a specific pod
```
### Managing resources

```
    kubectl apply -f <file>: create or update resources defined in a file
    kubectl delete <resource> <name>: delete a resource
    kubectl scale deployment <deployment-name> --replicas=<number>: scale a deployment to a specific number of replicas
```
### Debugging
```
    kubectl get events: list all events in the cluster
    kubectl describe node <node-name>: show details about a specific node
    kubectl describe service <service-name>: show details about a specific service
```
### Cluster administration
```
    kubectl config get-contexts: list available contexts
    kubectl config use-context <context-name>: switch to a specific context
    kubectl cluster-info: show information about the cluster
```
