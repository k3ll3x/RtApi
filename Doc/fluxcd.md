# FluxCD Cheatsheet

## Installation

To install fluxctl on Linux, you can use the following commands:

```bash

# Download the latest fluxctl binary
curl -sL https://github.com/fluxcd/flux/releases/latest/download/fluxctl_linux_amd64 -o fluxctl

# Move the binary to a directory
```

## Basic commands
```
    fluxctl list-workloads: list all workloads
    fluxctl list-images --workload=<workload>: list all images for a specific workload
    fluxctl release --workload=<workload> --update-all-images: update all images for a specific workload
```
## Managing resources
```
    fluxctl sync: synchronize the cluster with the Git repository
    fluxctl automate --workload=<workload>: enable automatic deployments for a specific workload
    fluxctl suspend --workload=<workload>: suspend automatic deployments for a specific workload
```
## Debugging
```
    fluxctl list-images --workload=<workload> --tag=<tag>: list all images for a specific tag
    fluxctl history --workload=<workload>: show deployment history for a specific workload
    fluxctl identity: show the public SSH key for Flux
```
## Managing Git repositories
```
    fluxctl list-sources: list all Git repositories being used by Flux
    fluxctl automate: enable automatic Git synchronization
    fluxctl suspend: suspend automatic Git synchronization
    fluxctl identity --namespace=<namespace>: show the public SSH key for Flux
```