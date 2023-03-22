# Kustomize Cheatsheet
## Installation

To install kustomize on Linux, you can use the following commands:

```bash

# Download the latest kustomize binary
curl -s "https://raw.githubusercontent.com/kubernetes-sigs/kustomize/master/hack/install_kustomize.sh" | bash

# Move the binary to a directory in your PATH
sudo mv kustomize /usr/local/bin/
```

### Basic commands
```
    kustomize build: generate the final YAML from the kustomization.yaml file
    kustomize build <directory>: generate the final YAML from the kustomization.yaml file in the specified directory
    kustomize build <URL>: generate the final YAML from the kustomization.yaml file at the specified URL
```
### Customizing resources
```
    kustomize edit add resource <resource.yaml>: add a resource to the kustomization.yaml file
    kustomize edit set image <image>:<tag>: set the image for a container in a deployment or pod
    kustomize edit add configmap <name> --from-literal=<key>=<value>: add a literal value to a configmap
```
### Managing overlays
```
    kustomize edit add patch <patch.yaml>: add a patch to the kustomization.yaml file
    kustomize edit add overlay <directory>: add an overlay to the kustomization.yaml file
    kustomize edit set namespace <namespace>: set the namespace for all resources in the kustomization.yaml file
```