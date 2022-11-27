#!/usr/bin/python3
import argparse
import sys

from collections import OrderedDict

import yaml


def combine(*configs):
    aggregate = {}
    # First pass, copy over version, etc
    for config in configs:
        if float(config["version"]) < 3:
            raise ValueError(
                "Only docker-compose files version 3 or greater are supported."
            )
        for section, value in config.items():
            if section not in ("networks", "services", "volumes"):
                if section not in aggregate or aggregate[section] != value:
                    # Version or some other key
                    aggregate[section] = value

    # Second pass, merge services/networks/volumes; preserves order.
    aggregate.update(
        {
            "services": {},
            "networks": {},
            "volumes": {},
        }
    )
    for config in configs:
        for section, value in config.items():
            if section in ("networks", "services", "volumes"):
                # A proper section
                for name, definition in value.items():
                    if name in aggregate[section]:
                        # merge service/network/volume definitions
                        merge(definition, aggregate[section][name])
                    else:
                        aggregate[section][name] = definition

    # Remove network/volumes if unused
    for section in ("networks", "volumes"):
        if not aggregate[section]:
            del aggregate[section]

    return aggregate


def merge(source, destination):
    """
    Deep merge dictionaries, join lists.
    """
    if isinstance(destination, dict):
        for key, value in source.items():
            if isinstance(value, dict):
                # get node or create one
                node = destination.setdefault(key, OrderedDict())
                merge(value, node)
            elif isinstance(destination.get(key), list) and isinstance(value, list):
                node = destination.setdefault(key, [])
                node += value
            else:
                destination[key] = value
    elif isinstance(destination, list):
        destination += source

    return destination


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Combine multiple docker-compose config files into a single config file."
    )
    parser.add_argument(
        "config_paths",
        metavar="PATH",
        type=str,
        nargs="+",
        help="Paths to docker-compose YAML config files",
    )

    args = parser.parse_args()
    configs = []
    for path in args.config_paths:
        with open(path, "r") as f:
            configs.append(yaml.load(f, Loader=yaml.Loader))
    config = combine(*configs)
    yaml.dump(config, sys.stdout)