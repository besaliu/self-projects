-- Your DDL statements go here; 
DROP TABLE IF EXISTS workspace_channels CASCADE;
DROP TABLE IF EXISTS channels CASCADE;
DROP TABLE IF EXISTS user_workspaces CASCADE;
DROP TABLE IF EXISTS users CASCADE;
DROP TABLE IF EXISTS workspaces CASCADE;

CREATE TABLE workspaces (
  id UUID PRIMARY KEY,
  data JSONB
);

CREATE TABLE channels (
  id UUID PRIMARY KEY,
  data JSONB
);

CREATE TABLE workspace_channels (
  workspace_id UUID REFERENCES workspaces(id) ON DELETE CASCADE,
  channel_id UUID REFERENCES channels(id) ON DELETE CASCADE,
  PRIMARY KEY (workspace_id, channel_id)
);

CREATE TABLE users (
  id UUID UNIQUE PRIMARY KEY,
  data JSONB
);

CREATE TABLE user_workspaces (
  user_id UUID REFERENCES users(id) ON DELETE CASCADE,
  workspace_id UUID REFERENCES workspaces(id) ON DELETE CASCADE,
  PRIMARY KEY (user_id, workspace_id)
);