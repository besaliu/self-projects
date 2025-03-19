import * as db from '../db.js';

/**
 *
 * @param {any} req req
 * @param {any} res res
 * @returns {any} return
 */
export async function userLogin(req, res) {
  const {email, password} = req.body;
  const result = await db.authenticateUser(email, password);

  if (!result) {
    return res.status(401).send();
  }

  // Return user info and token
  return res.json({
    token: result.token,
  });
}

/**
 * Get workspaces for a user by name
 * @param {any} req req
 * @param {any} res res
 * @returns {any} return
 */
export async function getWorkspaces(req, res) {
  const username = req.query.username;

  // Check if the authenticated user is requesting their own workspaces
  if (req.user.name !== username) {
    return res.status(403).send();
  }

  const workspaces = await db.userWorkspaces(username);

  return res.status(200).json(workspaces);
}

/**
 * Get workspaces for a user by name
 * @param {any} req req
 * @param {any} res res
 * @returns {any} return
 */
export async function getChannels(req, res) {
  const workspace = req.query.workspace;

  // First get the workspaces this user has access to
  const userWorkspaces = await db.userWorkspaces(req.user.name);

  // Check if the requested workspace is in the user's workspace list
  const hasAccess = userWorkspaces.some((w) => w.name === workspace);

  if (!hasAccess) {
    return res.status(403).send();
  }

  const channels = await db.selectChannels(workspace);

  return res.status(200).json(channels);
}

/**
 * Get a specific channel by ID including its messages
 * @param {any} req req
 * @param {any} res res
 * @returns {any} return
 */
export async function getChannelById(req, res) {
  const channelId = req.params.id;

  // First get all workspaces this user has access to
  const userWorkspaces = await db.userWorkspaces(req.user.name);

  // Get workspace IDs
  const workspaceIds = userWorkspaces.map((w) => w.workspace_id);

  // Check if the channel belongs to any of these workspaces
  // You'll need a new DB function for this check
  const hasAccess = await db
      .channelBelongsToUserWorkspaces(channelId, workspaceIds);

  if (!hasAccess) {
    return res.status(403).send();
  }

  const channelData = await db.getChannelById(channelId);

  return res.status(200).json(channelData);
}

/**
 *
 * @param {any} req req
 * @param {any} res res
 * @returns {any} any
 */
export async function getWorkspaceOwner(req, res) {
  const workspaceName = req.query.workspace;

  const userWorkspaces = await db.userWorkspaces(req.user.name);
  const hasAccess = userWorkspaces.some((w) => w.name === workspaceName);

  if (!hasAccess) {
    return res.status(403).send();
  }

  const owner = await db.getWorkspaceOwners(workspaceName);
  return res.status(200).json(owner);
}
