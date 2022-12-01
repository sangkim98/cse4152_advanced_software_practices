import pytest

import gym
from gym.wrappers import RecordEpisodeStatistics, VectorListInfo

ENV_ID = "CartPole-v1"
NUM_ENVS = 3
ENV_STEPS = 50
SEED = 42


def test_usage_in_vector_env():
    env = gym.make(ENV_ID, disable_env_checker=True)
    vector_env = gym.vector.make(ENV_ID, num_envs=NUM_ENVS, disable_env_checker=True)

    VectorListInfo(vector_env)

    with pytest.raises(AssertionError):
        VectorListInfo(env)


def test_info_to_list():
    env_to_wrap = gym.vector.make(ENV_ID, num_envs=NUM_ENVS, disable_env_checker=True)
    wrapped_env = VectorListInfo(env_to_wrap)
    wrapped_env.action_space.seed(SEED)
    _, info = wrapped_env.reset(seed=SEED, return_info=True)
    assert isinstance(info, list)
    assert len(info) == NUM_ENVS

    for _ in range(ENV_STEPS):
        action = wrapped_env.action_space.sample()
        _, _, dones, list_info = wrapped_env.step(action)
        for i, done in enumerate(dones):
            if done:
                assert "terminal_observation" in list_info[i]
            else:
                assert "terminal_observation" not in list_info[i]


def test_info_to_list_statistics():
    env_to_wrap = gym.vector.make(ENV_ID, num_envs=NUM_ENVS, disable_env_checker=True)
    wrapped_env = VectorListInfo(RecordEpisodeStatistics(env_to_wrap))
    _, info = wrapped_env.reset(seed=SEED, return_info=True)
    wrapped_env.action_space.seed(SEED)
    assert isinstance(info, list)
    assert len(info) == NUM_ENVS

    for _ in range(ENV_STEPS):
        action = wrapped_env.action_space.sample()
        _, _, dones, list_info = wrapped_env.step(action)
        for i, done in enumerate(dones):
            if done:
                assert "episode" in list_info[i]
                for stats in ["r", "l", "t"]:
                    assert stats in list_info[i]["episode"]
                    assert isinstance(list_info[i]["episode"][stats], float)
            else:
                assert "episode" not in list_info[i]
