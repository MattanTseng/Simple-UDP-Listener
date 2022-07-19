using System.Collections;
using System.Collections.Generic;
using System.Threading;
using System;
using UnityEngine;


// This dispatcher script is something that I found online. Seems to work well
// I don't fully understand how it works
public class Dispatcher : MonoBehaviour
{
    public static void RunAsync(Action action)
    {
        ThreadPool.QueueUserWorkItem(o => action());
    }

    public static void RunAsync(Action<object> action, object state)
    {
        ThreadPool.QueueUserWorkItem(o => action(o), state);
    }

    public static void RunOnMainThread(Action action)
    {
        lock (_backlog)
        {
            _backlog.Add(action);
            _queued = true;
        }
    }

    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.BeforeSceneLoad)]
    private static void Initialize()
    {
        if (_instance == null)
        {
            _instance = new GameObject("Dispatcher").AddComponent<Dispatcher>();
            DontDestroyOnLoad(_instance.gameObject);
        }
    }

    private void Update()
    {
        // if there is something to be inserted:
        if (_queued)
        {
            // stop main thread from receiving new tasks?
            lock (_backlog)
            {
                // insert our action into the main thread?
                var tmp = _actions;
                _actions = _backlog;
                _backlog = tmp;
                // show that we have no more actions to queue up
                _queued = false;
            }

            foreach (var action in _actions)
            {
                action();
            }
            _actions.Clear();
        }
    }

    // I think this stuff shows how the class operates?
    static Dispatcher _instance;
    static volatile bool _queued = false;
    static List<Action> _backlog = new List<Action>(8);
    static List<Action> _actions = new List<Action>(8);
}

